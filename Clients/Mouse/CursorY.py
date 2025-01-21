import sys
import pyautogui
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
class MulticastPingClient(DatagramProtocol):
    def startProtocol(self):
        self.transport.joinGroup(sys.argv[1])
        y1 = 0
        while True:
            y2 = pyautogui.position().y
            if y2 != y1:
                self.transport.write(bytes(str(y2)+".00", encoding='utf-8'), (sys.argv[1], int(sys.argv[2])))
            y1 = y2
    def datagramReceived(self, datagram, address):
        print(f"Datagram {repr(datagram)} received from {repr(address)}")