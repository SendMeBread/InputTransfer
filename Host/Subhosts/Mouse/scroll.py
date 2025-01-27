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
        if (int(datagram.decode('utf-8')) >= 1 or int(datagram.decode('utf-8') <= -1)) and not b".0" in datagram:
            mouse.scroll(None, int(datagram.decode('utf-8')))
reactor.listenMulticast(int(sys.argv[2]), MulticastInputs(), listenMultiple=True)
reactor.run()