Vagrant.configure("2") do |config|
    config.vm.box = "base"
    config.vm.provider :libvirt do |lv|
        lv.memory = "1024"
        lv.cpus = "2"
    end

    config.vm.define "sender" do |sender|
        sender.vm.box = "debian/bookworm64"
        sender.vm.hostname = "sender"
        sender.vm.network "private_network", ip: "192.168.56.110"
        sender.vm.provision "shell", path: "scripts/install.sh"
    end

    config.vm.define "target" do |target|
        target.vm.box = "debian/bookworm64"
        target.vm.hostname = "target"
        target.vm.network "private_network", ip: "192.168.56.111"
        target.vm.provision "shell", path: "scripts/install.sh"
        target.vm.provision "shell", path: "scripts/target.sh"
    end
end