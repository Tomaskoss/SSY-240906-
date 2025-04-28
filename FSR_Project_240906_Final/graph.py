import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

# Nastavenie sériového portu
ser = serial.Serial('COM4', 38400)  # Nahraďte 'COM3' správnym portom

# Vytvorenie grafov
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))
xs = []
ys = []
diffs = []

# Funkcia na aktualizáciu grafov
def animate(i, xs, ys, diffs):
    try:
        # Čítanie dát zo sériového portu
        line = ser.readline().decode('utf-8').strip()
        if line.startswith("ADC Value:"):
            value = int(line.split(":")[1])
            xs.append(len(xs))
            ys.append(value)

            # Výpočet rozdielov
            if len(ys) > 1:
                diff = ys[-1] - ys[-2]
                diffs.append(diff)
            else:
                diffs.append(0)  # Pre prvú hodnotu rozdiel je 0

            # Obmedzenie počtu bodov v grafoch
            xs = xs[-100:]
            ys = ys[-100:]
            diffs = diffs[-100:]

            # Vykreslenie grafov
            ax1.clear()
            ax1.plot(xs, ys)
            ax1.set_xlabel('Čas')
            ax1.set_ylabel('Hodnota ADC')
            ax1.set_title('Live graf hodnôt ADC')
            ax1.set_yticks(range(0, max(ys)+50, 50))  # Nastavenie osy Y po 25

            ax2.clear()
            ax2.plot(xs, diffs)
            ax2.set_xlabel('Čas')
            ax2.set_ylabel('Rozdiel ADC')
            ax2.set_title('Live graf rozdielov hodnôt ADC')
            ax2.set_yticks(range(-500,max(ys)+50,100))

        # Úprava rozloženia grafov
        plt.tight_layout()

    except ValueError:
        pass
    except serial.SerialException as e:
        print(f"Chyba sériového portu: {e}")
        exit()

# Animácia grafov
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys, diffs), interval=100)

# Zobrazenie grafu
plt.show(block=False)

# Uzavretie okna a ukončenie programu
while plt.fignum_exists(fig.number):
    plt.pause(0.1)

ser.close()  # Uzatvorenie sériového portu