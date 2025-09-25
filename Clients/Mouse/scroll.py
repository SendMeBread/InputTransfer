import sys
from pynput import mouse
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
class MulticastInputs(DatagramProtocol):
    def __init__(self):
        super().__init__()
        self.listener = None

    def startProtocol(self):
        self.transport.setTTL(5)
        self.transport.joinGroup(sys.argv[1])
        self.listener = mouse.Listener(on_scroll=self.on_scroll)
        self.listener.start()

    def on_scroll(self, x, y, dx, dy):
        if self.transport:
            y_scroll = y + dy
            try:
                self.transport.write(bytes(str(y_scroll) + " scrolled", encoding="utf-8"), (sys.argv[1], int(sys.argv[2])))
            except Exception as e:
                print(f"Error sending scroll event: {e}")
        else:
            print("Transport not ready, scroll event not sent.")

    def datagramReceived(self, datagram, address):
        print(f"Datagram {repr(datagram)} received from {repr(address)}")

protocol = MulticastInputs()
reactor.listenMulticast(int(sys.argv[2]), protocol, listenMultiple=True)
reactor.run()