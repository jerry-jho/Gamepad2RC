import time
import serial
ser = serial.Serial('COM1', 9600)  # open serial port
print(ser.name)         # check which port was really used
print(ser.is_open)
# >>> ser.write(b'hello')     # write a string
# >>> ser.close()             # close port
print(ser.write(b'\x84'))
ser.flush()
time.sleep(1)
# ser.close()
# exit()

import pygame
pygame.init()
pygame.joystick.init()


# -------- 主程序循环 -----------

    # 获取操纵杆的数量。
joystick_count = pygame.joystick.get_count()
i = 0
joystick = pygame.joystick.Joystick(i)
joystick.init()
name = joystick.get_name()
print(name)
last_b = None
last_s = None
while True:
    for event in pygame.event.get():
        if event.type == pygame.JOYAXISMOTION:
            if event.axis == 0:
                axis = joystick.get_axis(0)
                if axis > 0:
                    b = b'\x80'
                    if axis > 0.9:
                        b = b'\x84'
                    elif axis > 0.75:
                        b = b'\x83'
                    elif axis > 0.5:
                        b = b'\x82'
                    elif axis > 0.25:
                        b = b'\x81'
                    if last_b is None or b != last_b:
                        print(axis, b)
                        ser.write(b)
                        last_b = b
                else:
                    axis = -1 * axis
                    b = b'\xA0'
                    if axis > 0.9:
                        b = b'\xA4'
                    elif axis > 0.75:
                        b = b'\xA3'
                    elif axis > 0.5:
                        b = b'\xA2'
                    elif axis > 0.25:
                        b = b'\xA1'
                    if last_b is None or b != last_b:
                        print(axis, b)
                        ser.write(b)
                        last_b = b
            elif event.axis == 1:
                axis = joystick.get_axis(1)
                axis = int(axis * 32)
                #print(axis)
                s = b'\xC0'
                if axis < 0:
                    axis = -1 * axis
                    if axis > 30:
                        s = b'\xCF'
                    elif axis > 22:
                        s = b'\xCD'
                    elif axis > 15:
                        s = b'\xC9'
                    elif axis > 7:
                        s = b'\xC5'
                if last_s is None or last_s != s:
                    print(hex(ord(s)))
                    ser.write(s)
    pygame.time.wait(0)