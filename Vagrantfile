Vagrant.configure("2") do |config|
    config.vm.provider "virtualbox" do |v|
      v.memory = 2048
      v.cpus = 2
    end
    config.vm.box = "ubuntu/bionic64"
        config.vm.synced_folder "./", "/home/vagrant/oci-containers"
        config.vm.provision "shell" do |s|
        s.path = "install-deps.sh"
        s.privileged = true
    end
end
