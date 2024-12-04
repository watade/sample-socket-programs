# sample-socket-programs

TCP サーバー/クライアントを題材としたソケット通信のサンプルプログラム

## 動作環境

Ubuntu 20.04.6 LTS

## 準備

### Linux 仮想ネットワークの作成

1. server, client, router の 3 つの NameSpace からなる仮想ネットワークを作成する.
   ```
   sudo ./create_vnet.sh
   ```
1. client から server に ping が通るかを確かめる.
   ```
   sudo ip netns exec client ping 192.168.100.10
   ```
1. (プログラム実行終了後) 再起動すれば仮想ネットワークの設定は消えるが, 以下で削除も可能
   ```
   sudo ./delete_vnet.sh
   ```

### 実行ファイルのビルド

1. 実行ファイル(tcp_server, tcp_client, multi_tcp_server)をビルド
   ```
   make
   ```
1. (プログラム実行終了後) 中間ファイル, 実行ファイルを消去
   ```
   make clean
   ```

## プログラムの実行

### TCP サーバー/クライアントプログラムの実行

1. server 上で tcp_server を実行し, TCP サーバーを起動
   ```
   sudo ip netns exec server ./tcp_server
   ```
1. 別のターミナルを開き, client 上で tcp_client を実行
   ```
   sudo ip netns exec client ./tcp_client
   ```
1. 別ターミナルで tcpdump コマンドを使ってパケットキャプチャしながら動かし, 出力ファイルを Wireshark で表示
   ```
   sudo ip netns exec router tcpdump -i veth-router1 -w /tmp/tcpdump-file.pcap
   ```

### TCP サーバーの I/O 多重化

1. server 上で multi_tcp_server を実行し, TCP サーバーを起動
   ```
   sudo ip netns exec server ./multi_tcp_server
   ```
1. 別のターミナルを複数開き, client 上で nc コマンドによる接続とメッセージ送信を行う
   ```
   sudo ip netns exec client nc 192.168.100.10 8000
   Hello! (適当なメッセージ)
   ```
