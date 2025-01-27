import sys
from pynput.mouse import Controller
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
mouse = Controller()
class MulticastInputs(DatagramProtocol):
    def startProtocol(self):
        self.transport.setTTL(5)
        self.transport.joinGroup(sys.argv[1])
    def datagramReceived(self, datagram, address):
        if b".00" in datagram:
            X = datagram.decode('utf-8')
            mouse.move(float(X), None)
reactor.listenMulticast(int(sys.argv[2]), MulticastInputs(), listenMultiple=True)
reactor.run()