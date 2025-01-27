import sys
from pynput.mouse import Controller
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
mouse = Controller()
class MulticastPingClient(DatagramProtocol):
    def startProtocol(self):
        self.transport.joinGroup(sys.argv[1])
        y1, x1 = mouse.position
        while True:
            y2, x2 = mouse.position
            if y2 != y1 or x2 != x1:
                self.transport.write(bytes(str(y2),",",str(x2)), encoding='utf-8'), (sys.argv[1], int(sys.argv[2]))
            y1, x1 = y2, x2
    def datagramReceived(self, datagram, address):
        print(f"Datagram {repr(datagram)} received from {repr(address)}")
reactor.listenMulticast(int(sys.argv[2]), MulticastPingClient(), listenMultiple=True)
reactor.run()