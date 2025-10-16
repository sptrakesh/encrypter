# Python Client
A simple client for interacting with the service written in python.

```shell
from encrypter import Client as _EClient

async with _EClient(host="encrypter") as client:
    text = "https://github.com/sptrakesh/encrypter/blob/master/client/python/features/steps/service.py"
    encrypted = await client.encrypt(data=text)
    decrypted = await client.decrypt(data=encrypted)
    assert(encrypted == decrypted)
```