# remote-firewall
This is a customed server firewall that can prevent packets from certain ip address from reaching certain port of the server. This firewall can also be configured to block the server from reaching external website. This firewall can be loaded to a Linux kernel as a module. It can be loaded locally or remotely vias ssh.

## Prerequist:
To load the firewall remotely, have ssh installed in both the server and the remote machine.

## Usage
### Loaded locally
Clone this repo to your server. Go to the directory of this repo in the server's terminal and run the batch file local-add:
* $ sudo ./local-add

Configure the port number, the ip address, and the block external website option in the box. The firewall will be loaded to your server's kernel. To unload the firewall, run the following batch file by:
* $ sudo ./local-remove

### Loaded remotely vias ssh

Clone this repo to remote machine. Go to the directory of this repo in the terminal and run the batch file remote-add with three additional parameters:
* $ sudo ./remote-add [host-name@host-address] [block external webs? 1=yes/0=no] [ip address in string] [port number]
* ie.$ sudo ./remote-add foo@123.123.123.123 0 "198.172.1.0" 80

Configure the port number, the ip address, and the block external website option in the corresponding places. The firewall will be loaded to your remote server's kernel. To unload the firewall, run the following batch file by:
* $ sudo ./remote-remove
