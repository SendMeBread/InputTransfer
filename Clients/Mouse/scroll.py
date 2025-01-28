import sys
from pynput import mouse
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
class MulticastInputs(DatagramProtocol):    
    def startProtocol(self):
        self.transport.setTTL(5)
        self.transport.joinGroup(sys.argv[1])
    def on_scroll(self, x, y, dx, dy):
        self.y_scroll = dy
        self.transport.write(bytes(str(self.y_scroll)+" scrolled", encoding="utf-8"), (sys.argv[1], int(sys.argv[2])))
    def datagramReceived(self, datagram, address):
        print(f"Datagram {repr(datagram)} received from {repr(address)}")
with mouse.Listener(on_scroll=MulticastInputs.on_scroll)
reactor.listenMulticast(int(sys.argv[2]), MulticastInputs(), listenMultiple=True)
reactor.run()