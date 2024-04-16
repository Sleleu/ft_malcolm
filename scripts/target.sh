#!/bin/bash

cat << EOF > /home/vagrant/log_arp.sh
#!/bin/bash
LOG_FILE="/home/vagrant/arp_log.txt"
echo "\$(date '+%Y-%m-%d %H:%M:%S') - ARP Table:" >> "\$LOG_FILE"
sudo arp -a >> "\$LOG_FILE"
echo "-------------------------------------------------------" >> "\$LOG_FILE"
EOF


chmod a+x /home/vagrant/log_arp.sh


(crontab -l 2>/dev/null; echo "* * * * * /home/vagrant/log_arp.sh") | crontab -