import sys
from pynput.mouse import Controller
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
mouse = Controller()
class MulticastPingClient(DatagramProtocol):
    def startProtocol(self):
        self.transport.joinGroup(sys.argv[1])
        self.last_position = mouse.position
        self.poll_mouse()

    def poll_mouse(self):
        x1, y1 = self.last_position
        x2, y2 = mouse.position
        if (x2, y2) != (x1, y1):
            try:
                self.transport.write(bytes(f"{x2},{y2}", encoding='utf-8'), (sys.argv[1], int(sys.argv[2])))
            except Exception as e:
                print(f"Error sending multicast: {e}")
            self.last_position = (x2, y2)
        reactor.callLater(0.025, self.poll_mouse)

    def datagramReceived(self, datagram, address):
        print(f"Datagram {repr(datagram)} received from {repr(address)}")
reactor.listenMulticast(int(sys.argv[2]), MulticastPingClient(), listenMultiple=True)
reactor.run()