import platform
import uuid
import hashlib
import subprocess

def get_cpu():
    return platform.processor()

def get_mac():
    mac = uuid.getnode()
    return ':'.join(['{:02x}'.format((mac >> elements) & 0xff)
                     for elements in range(0,2*6,2)][::-1])

def get_disk():
    try:
        output = subprocess.check_output("wmic diskdrive get size", shell=True)
        return output.decode()
    except:
        return "Unknown"

cpu = get_cpu()
mac = get_mac()
disk = get_disk()

fingerprint_string = cpu + mac + disk

fingerprint = hashlib.sha256(fingerprint_string.encode()).hexdigest()

print("CPU:", cpu)
print("MAC:", mac)
print("Disk:", disk)
print("System Fingerprint:", fingerprint)
