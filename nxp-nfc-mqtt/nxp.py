import nxppy
import time

m = nxppy.Mifare()

while True:
    try:
        uid = m.select()
        print(uid)
    except nxppy.SelectError:
        pass

    time.sleep(1)
