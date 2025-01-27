import sys
from pynput.mouse import Controller
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
mouse = Controller()
class MulticastPingClient(DatagramProtocol):
    def startProtocol(self):
        self.transport.joinGroup(sys.argv[1])
        y1 = mouse.position[1]
        while True:
            y2 = mouse.position[1]
            if y2 != y1:
                self.transport.write(bytes(str(y2)+".01", encoding='utf-8'), (sys.argv[1], int(sys.argv[2])))
            y1 = y2
    def datagramReceived(self, datagram, address):
        print(f"Datagram {repr(datagram)} received from {repr(address)}")
reactor.listenMulticast(int(sys.argv[2]), MulticastPingClient(), listenMultiple=True)
reactor.run()