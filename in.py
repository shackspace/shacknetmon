#!/usr/bin/env python3

import subprocess
import locale
import time
import serial

ser = serial.Serial('/dev/ttyACM0')
max_bandwidth = 50

mib = ".1.3.6.1.2.1.2.2.1.10" #ifInOctets
delta_t = 4
encoding = locale.getdefaultlocale()[1]
snmpwalk = ["snmpwalk", "-v", "2c", "-c", "shack", "10.0.0.3"]
#interfaces = {35: 'vlan10', 36: 'vlan100', 37:'vlan200', 38:'vlan210', 39:'vlan220', 40:'vlan300'}
interfaces = {5: 'firewall'}

def main():
    oldest = get_counters(get_snmp())
    time.sleep(delta_t)
    newest = get_counters(get_snmp())
    delta = newest[5] - oldest[5]
    result = delta / delta_t
    result /= 1024 #kbps
    result /= 128 #mbps
    percentage = (result / max_bandwidth) * 100
    percentage = int(percentage)
    percentage = str(percentage)
    data = percentage + '\n'
    ser.write(bytes(data, 'UTF-8'))
    ser.close()



def get_counters(snmp):
    counters = {}
    for el in snmp:
        el = el.strip()
        el = el.decode(encoding)
        el = el.split(' = ')
        interface = el[0].split('.')[1]
        interface = int(interface)
        counter = el[1].split(': ')[1]
        counter = int(counter)
        counters[interface] = counter
    return counters




def get_snmp():
    cmd = snmpwalk
    cmd.append(mib)
    with subprocess.Popen(cmd, stdout=subprocess.PIPE) as pro:
        out = pro.stdout.readlines()
    return out

if __name__ == "__main__":
    main()
