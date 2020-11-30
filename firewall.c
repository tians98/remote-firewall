#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/inet.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL"); 
  
MODULE_AUTHOR("Shiliang Tian"); 
  
MODULE_DESCRIPTION("Customed Linux Kernel Firewall"); 
  
MODULE_VERSION("1.1");

// input parameters to the module indicating ip address, port number, and block
static char *myIp = "";
static int myPort = 2;
static int blockExternal = 3;

// initialize packet hooks
static struct nf_hook_ops ipFilterHook;
static struct nf_hook_ops portFilterHook;
static struct nf_hook_ops externalFilterHook;

// extract input parameter
module_param(myIp, charp, 0000);
MODULE_PARM_DESC(myIp, "A string");
module_param(myPort, int, 0000);
MODULE_PARM_DESC(myPort, "An integer");
module_param(blockExternal, int, 0000);
MODULE_PARM_DESC(blockExternal, "An integer");

//Block packet from customed IP address
unsigned int ipFilter(void *priv, struct sk_buff *skb, const struct nf_hook_state *state){
    	struct iphdr *iph;
    	
	__u32 src_ip; 	//Source IP Address
    	__u32 dst_ip; 	//Destination IP Address

	iph = ip_hdr(skb);

	src_ip = iph->saddr;
   	dst_ip = iph->daddr;

	//char sIp1[4]
	//char sIp2[4]
	//char sIp3[4]
	//char sIp4[4]
	//itoa(myIp1, sIp1, 10);
	//itoa(myIp2, sIp2, 10);
	//itoa(myIp3, sIp3, 10);
	//itoa(myIp4, sIp4, 10);

	//Block packet from customed IP address
	if (src_ip == in_aton(myIp)) {
		printk(KERN_INFO "Blocking packet from IP: %s\n", 				
			myIp);
			
        	return NF_DROP;
    	}

	printk(KERN_INFO "Allow Packet from IP: %d.%d.%d.%d\n", 				
			((unsigned char *)&src_ip)[0],
			((unsigned char *)&src_ip)[1],
			((unsigned char *)&src_ip)[2],
			((unsigned char *)&src_ip)[3]);
	return NF_ACCEPT;
}

//Block packet to customed port
unsigned int portFilter(void *priv, struct sk_buff *skb, const struct nf_hook_state *state){
    	struct iphdr *iph;
    	struct tcphdr *tcph;

	iph = ip_hdr(skb);
    	tcph = (void *)iph+iph->ihl*4;

	//Block FTP Server from Client
	if (tcph->dest == htons(0)) {
		printk(KERN_INFO "Blocking packet to Port: %i\n", 				\
			myPort);
        	return NF_DROP;
    	}

	printk(KERN_INFO "Packet Accepted.\n");
	return NF_ACCEPT;
}

//Block packets from external website
unsigned int externalFilter(void *priv, struct sk_buff *skb, const struct nf_hook_state *state){
    	struct iphdr *iph;
    	struct tcphdr *tcph;

	iph = ip_hdr(skb);
    	tcph = (void *)iph+iph->ihl*4;
	
	//Block external website
	if (tcph->dest == htons(80) || tcph->dest == htons(443)) {
		printk(KERN_INFO "Blocking external websites");
        	return NF_DROP;
    	}

	printk(KERN_INFO "Packet accepted.\n");
	return NF_ACCEPT;
}

int setUpFilter(void) {
    printk(KERN_ALERT "%i", myPort);
    printk(KERN_ALERT "%s", myIp);
    printk(KERN_ALERT "%i", blockExternal);
   
    ipFilterHook.hook = ipFilter;
    ipFilterHook.hooknum = NF_INET_PRE_ROUTING;
    ipFilterHook.pf = PF_INET;
    ipFilterHook.priority = NF_IP_PRI_FIRST;
    nf_register_net_hook(&init_net,&ipFilterHook);
    printk(KERN_INFO "Registered an ip filter.\n");

    portFilterHook.hook = portFilter;
    portFilterHook.hooknum = NF_INET_PRE_ROUTING;
    portFilterHook.pf = PF_INET;
    portFilterHook.priority = NF_IP_PRI_FIRST;
    nf_register_net_hook(&init_net,&portFilterHook);
    printk(KERN_INFO "Registered a port filter.\n");

    if(blockExternal == 1){
	externalFilterHook.hook = externalFilter;
	externalFilterHook.hooknum = NF_INET_POST_ROUTING;
	externalFilterHook.pf = PF_INET;
	externalFilterHook.priority = NF_IP_PRI_FIRST;
	nf_register_net_hook(&init_net,&externalFilterHook);
	printk(KERN_INFO "Registered a filter to external website.\n");
    }
    

    return 0;
}

void removeFilter(void) {
    nf_unregister_net_hook(&init_net,&ipFilterHook);
    printk(KERN_INFO "Ip filter is removed.\n");

    nf_unregister_net_hook(&init_net,&portFilterHook);
    printk(KERN_INFO "Port filter is removed.\n");

    nf_unregister_net_hook(&init_net,&externalFilterHook);
    printk(KERN_INFO "External Website filter is removed.\n");
}

module_init(setUpFilter);
module_exit(removeFilter);
