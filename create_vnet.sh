# vethペア作成
ip link add name veth-router1 type veth peer name veth-server
ip link add name veth-router2 type veth peer name veth-client
# namespace作成
ip netns add server
ip netns add client
ip netns add router
# namespaceにインターフェースを割り当てる
ip link set veth-server netns server
ip link set veth-client netns client
ip link set veth-router1 netns router
ip link set veth-router2 netns router
# インターフェースにアドレスを割り当てる
ip netns exec server ip addr add 192.168.100.10/24 dev veth-server
ip netns exec client ip addr add 192.168.200.10/24 dev veth-client
ip netns exec router ip addr add 192.168.100.1/24 dev veth-router1
ip netns exec router ip addr add 192.168.200.1/24 dev veth-router2
# インターフェースをUP
ip netns exec server ip link set veth-server up
ip netns exec client ip link set veth-client up
ip netns exec router ip link set veth-router1 up
ip netns exec router ip link set veth-router2 up
# ルーティングの設定
ip netns exec server ip route add 192.168.200.0/24 via 192.168.100.1 dev veth-server
ip netns exec client ip route add 192.168.100.0/24 via 192.168.200.1 dev veth-client
