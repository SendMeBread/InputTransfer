import sys
from pynput import mouse
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
class MulticastInputs(DatagramProtocol):
    def on_scroll(self, x, y, dx, dy):
        return dy
    def startProtocol(self):
        self.transport.setTTL(5)
        self.transport.joinGroup(sys.argv[1])
        while True:
            if self.on_scroll != 0:
                self.transport.write(bytes(str(self.on_scroll), encoding="utf-8"), (sys.argv[1], int(sys.argv[2])))
                self.on_scroll = 0
    def datagramReceived(self, datagram, address):
        print(f"Datagram {repr(datagram)} received from {repr(address)}")
with mouse.Listener(on_move=None, on_click=None, on_scroll=MulticastInputs.on_scroll) as mListener:
    mListener.join()
reactor.listenMulticast(int(sys.argv[2]), MulticastInputs(), listenMultiple=True)
reactor.run()