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
        if b".01" in datagram:
            Y = datagram.decode('utf-8')
            mouse.move(None, float(Y))
reactor.listenMulticast(int(sys.argv[2]), MulticastInputs(), listenMultiple=True)
reactor.run()