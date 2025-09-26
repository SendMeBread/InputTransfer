import sys
import keyboard
import asyncio
from twisted.internet import reactor
from twisted.internet.protocol import DatagramProtocol
class MulticastPingClient(DatagramProtocol):
    async def runKeys(self):
        await self.w_send()
        await self.a_send()
        await self.s_send()
        await self.d_send()
        await reactor.callLater(0.025, self.runKeys)
    async def w_send(self):
        if keyboard.is_pressed('w'):
            self.transport.write(b"w", (sys.argv[1], int(sys.argv[2])))
    async def a_send(self):
        if keyboard.is_pressed('a'):
            self.transport.write(b"a", (sys.argv[1], int(sys.argv[2])))
    async def d_send(self):
        if keyboard.is_pressed('d'):
            self.transport.write(b"d", (sys.argv[1], int(sys.argv[2])))
    async def s_send(self):
        if keyboard.is_pressed('s'):
            self.transport.write(b"s", (sys.argv[1], int(sys.argv[2])))
    async def startProtocol(self):
        self.transport.joinGroup(sys.argv[1])
if __name__ == "__main__":
    asyncio.run(MulticastPingClient().runKeys())
reactor.listenMulticast(int(sys.argv[2]), MulticastPingClient(), listenMultiple=True)
reactor.run()