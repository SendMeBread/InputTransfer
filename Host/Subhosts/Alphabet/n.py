import sys
import pyautogui
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
class MulticastInputs(DatagramProtocol):
    def startProtocol(self):
        self.transport.setTTL(5)
        self.transport.joinGroup(sys.argv[1])
    def datagramReceived(self, datagram, address):
        if b"n" in datagram:
            pyautogui.press("n")
reactor.listenMulticast(int(sys.argv[2]), MulticastInputs(), listenMultiple=True)
reactor.run()