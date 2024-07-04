from asyncio import open_connection
from sys import byteorder

from logger import log as _log


class Client:
    def __init__(self, host: str, port: int = 2030):
        assert host
        self.__host = host
        assert port > 0
        self.__port = port

    async def _async_init(self):
        self.__reader, self.__writer = await open_connection(host=self.__host, port=self.__port)
        _log.info(f"Connected to {self.__host}:{self.__port}")
        return self

    def __await__(self):
        return self._async_init().__await__()

    async def __aenter__(self):
        await self._async_init()
        return self

    async def __aexit__(self, exc_type, exc_value, traceback):
        await self.close()

    async def __execute(self, buf: bytes) -> bytes:
        lv = len(buf).to_bytes(length=4, byteorder=byteorder)
        ba = b''.join([lv, buf])
        _log.info(f"Writing {len(ba)} bytes to server.")

        self.__writer.write(ba)
        await self.__writer.drain()

        _log.info("Reading response size to 4 byte array")
        lv = await self.__reader.readexactly(4)
        l = int.from_bytes(bytes=lv, byteorder=byteorder)
        _log.info(f"Response size: {l}")

        return await self.__reader.readexactly(l)

    async def encrypt(self, data: str) -> str:
        assert len(data) > 0
        _log.info("Encrypting data")
        b = await self.__execute(f"e {data}".encode("utf-8"))
        return b.decode("utf-8")

    async def encrypt_bytes(self, data: bytes) -> bytes:
        assert len(data) > 0
        _log.info("Encrypting bytes")
        return await self.__execute(b''.join(["e ".encode("utf-8"), data]))

    async def decrypt(self, data: str) -> str:
        assert len(data) > 0
        _log.info("Decrypting data")
        b = await self.__execute(f"d {data}".encode("utf-8"))
        return b.decode("utf-8")

    async def decrypt_bytes(self, data: bytes) -> bytes:
        assert len(data) > 0
        _log.info("Decrypting bytes")
        return await self.__execute(b''.join(["d ".encode("utf-8"), data]))

    async def close(self):
        self.__writer.close()
        await self.__writer.wait_closed()
        _log.info(f"Disconnected from {self.__host}:{self.__port}")