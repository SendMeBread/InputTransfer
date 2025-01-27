import sys
from pynput import mouse
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
class MulticastInputs(DatagramProtocol):    
    def startProtocol(self):
        self.transport.setTTL(5)
        self.transport.joinGroup(sys.argv[1])
        with mouse.Events() as events:
            for event in events:
                try:
                    if event.dy != 0:
                        self.transport.write(bytes(str(event.dy), encoding="utf-8"), (sys.argv[1], int(sys.argv[2])))
                except:
                    continue
    def datagramReceived(self, datagram, address):
        print(f"Datagram {repr(datagram)} received from {repr(address)}")
reactor.listenMulticast(int(sys.argv[2]), MulticastInputs(), listenMultiple=True)
reactor.run()