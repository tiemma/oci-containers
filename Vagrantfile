Vagrant.configure("2") do |config|
    config.vm.box = "ubuntu/bionic64"
        config.vm.synced_folder "../docker", "/home/vagrant/docker"
        config.vm.provision "shell" do |s|
        s.path = "install-deps.sh"
        s.privileged = true
    end
end