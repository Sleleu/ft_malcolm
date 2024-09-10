## About the project

*DISCLAIMER: This project is for educational purposes only.*

The goal of this project is to implement an ARP spoofing tool in C. 
The program intercepts ARP requests from a target and sends a spoofed ARP reply, associating a fake IP address with a specific MAC address. 
This helps in understanding how the ARP protocol works at the network layer and how it can be exploited for network attacks.

### Understanding the ARP Protocol and its vulnerabilities

The Address Resolution Protocol (ARP) is a network protocol used to map IP addresses to MAC addresses in local networks, enabling devices to communicate on the same network segment.

Although devices are assigned IP addresses for network-level communication, the actual communication on a local network happens through MAC addresses, which are unique identifiers assigned to network interfaces. Since computers communicate at the **Data Link Layer** (Layer 2 of the OSI model) using MAC addresses, ARP plays a role in linking an IP address (used at **Layer 3, the Network Layer**) to a device's MAC address.

Indeed, when a device wants to communicate with another device on the same network, it needs to know the target's MAC address. If it only has the IP address, ARP is used to resolve this by broadcasting a request on the network. The device with the corresponding IP address responds with its MAC address, and this mapping is stored in an ARP cache for future use, allowing devices to directly communicate using their MAC addresses :

![ARP_protocol](https://github.com/user-attachments/assets/3c09a576-407d-4484-bec9-b76a7e10891c)
*Source: [GeekFlare](https://geekflare.com/fr/address-resolution-protocol/)*

Despite its importance, ARP is inherently vulnerable due to its lack of authentication mechanisms, making it susceptible to attacks such as ARP spoofing or ARP poisoning. These attacks exploit the trust-based nature of the protocol, allowing an attacker to send falsified ARP replies that associate an incorrect MAC address with an IP address. This can lead to **Man-in-the-Middle (MitM)** attacks, where the attacker intercepts or alters network traffic, or **Denial of Service (DoS)** by corrupting the target’s ARP cache.

### Structure of an ARP Packet

ARP Packet is consists of **Ethernet Frame Header** and **ARP Header**. The length of the Ethernet Frame Header is 14 bytes, and 28 bytes for ARP Header.

![arp-packet-format-ipcisco](https://github.com/user-attachments/assets/8cadf049-8efb-438c-8042-21e32c9a87c3)\
*Source: [IPCisco: Address Resolution Protocol (ARP)](https://ipcisco.com/lesson/address-resolution-protocol-arp/)*


Here is the structure I created to send and receive ARP packets for this project:

```C
/* ARP PACKET */
typedef struct __attribute__((packed)) s_arp_packet {
    /* ETHERNET HEADER */
	u_int8_t    h_target[ETH_ALEN];
	u_int8_t    h_src[ETH_ALEN];
	u_int16_t   h_proto;

    /* ARP HEADER */
    u_int16_t   hardware_type;
    u_int16_t   proto_type;
    u_int8_t hardware_addr_len;
    u_int8_t proto_addr_len;
    u_int16_t operation;
    u_int8_t mac_src[ETH_ALEN];
    u_int32_t ip_src;
    u_int8_t mac_target[ETH_ALEN];
    u_int32_t ip_target;
} t_arp_packet;
```
The `__attribute__((packed))` is used to ensure there’s no padding in the structure, allowing the ARP packet to be constructed and sent exactly as required by the protocol's specification.


## Installation and usage

Git clone and install the repository:

```bash
git clone https://github.com/sleleu/ft_malcolm
cd ft_malcolm
git submodule update --init --recursive
make -j
```

To start the VMs with the Vagrantfile, run the following command:

```bash
vagrant up
```
(Ensure that Vagrant is installed for using the VMs. The provider used in this project is `libvirt`.)

Once the VMs are up, you can connect to each VM via SSH using the following commands:

- To connect to the sender VM:

```bash
vagrant ssh sender
```

To connect to the target VM:

```bash
vagrant ssh target
```

Run the program to perform ARP spoofing by specifying the source and target IP and MAC addresses as follows:

```bash
sudo ./ft_malcolm <IP-src> <MAC-src> <IP-target> <MAC-target> [OPTION -v -f]
```
The `ft_malcolm` binary can be found directly in the Vagrant VMs within the `/vagrant` folder. This is a directory shared by default between the different virtual machines launched in a Vagrant box.

To perform ARP spoofing, set :
- `<IP-src>` to the IP address you want to spoof,
- `<MAC-src>` to your own MAC address,
- `<IP-target>` and `<MAC-target>` to the target machine’s IP and MAC address.

Here is the main logic : 

```C
while (42) {
  if (receive_arp_request()) {
    printf("Now sending an ARP reply to the target address with spoofed source, please wait...\n");
    send_arp_packet(&packet, socket_address);
    if (g_data.verbose)
      print_packet(packet);
    printf("Sent an ARP reply packet, you may now check the arp table on the target.\n");
  }
}
```

The program will wait for the next ARP request from the target and then send an ARP reply with the spoofed information. This will result in the target’s ARP table being updated with the spoofed MAC address.


By using the flag `-v` (verbose), you can view the detailed content of the ARP packets that are received and sent to the target:

![ARP_REPLY](https://github.com/user-attachments/assets/2365d22c-3ce5-46eb-9b2c-ec5542b1bfb6)


## Usefull resources

- ARP protocol : https://man7.org/linux/man-pages/man7/arp.7.html
- Structure of an ARP packet : 
  - https://stackoverflow.com/questions/41403445/how-to-structure-and-arp-request-packet-in-c
  - https://stackoverflow.com/questions/16710040/arp-request-and-reply-using-c-socket-programming
- RFC 826 for ARP: https://datatracker.ietf.org/doc/html/rfc826
- ARP header: https://fr.wikipedia.org/wiki/Address_Resolution_Protocol
- Article on how ARP works : https://www.frameip.com/entete-arp/
- Install Vagrant : https://developer.hashicorp.com/vagrant/install
- Nice tuto : https://www.youtube.com/watch?v=16VO0wc8HfM
