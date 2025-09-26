import sys
from pynput import keyboard
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
class MulticastPingClient(DatagramProtocol):
    def startProtocol(self):
        self.transport.joinGroup(sys.argv[1])
        while True:
            if keyboard.is_pressed('p'):
                self.transport.write(b"p", (sys.argv[1], int(sys.argv[2])))
reactor.listenMulticast(int(sys.argv[2]), MulticastPingClient(), listenMultiple=True)
reactor.run