import sys
from pynput.mouse import Controller
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
mouse = Controller()
class MulticastPingClient(DatagramProtocol):
    def startProtocol(self):
        self.transport.joinGroup(sys.argv[1])
        x1 = mouse.position[0]
        while True:
            x2 = mouse.position[0]
            if x2 != x1:
                self.transport.write(bytes(str(x2)+".00", encoding='utf-8'), (sys.argv[1], int(sys.argv[2])))
            x1 = x2
    def datagramReceived(self, datagram, address):
        print(f"Datagram {repr(datagram)} received from {repr(address)}")
reactor.listenMulticast(int(sys.argv[2]), MulticastPingClient(), listenMultiple=True)
reactor.run()