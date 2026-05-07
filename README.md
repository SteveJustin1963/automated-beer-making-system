Making beer at home follows a logical progression of biology and chemistry. When you introduce an **Arduino Nano**, you’re essentially replacing human guesswork with digital precision.

Here are the stages of brewing and how the Nano integrates into each.

---

### 1. The Mash (Conversion)

**The Goal:** Soak crushed malted grain in hot water (usually between **63°C and 68°C**) to trigger enzymes that turn grain starches into fermentable sugar.

* **Nano’s Job:** * **Temperature Monitoring:** Reads the DS18B20 probe every second.
* **Heat Maintenance:** Uses **PID Logic** to pulse a Solid State Relay (SSR). It ensures the water doesn't vary by more than 0.5°C.
* **Recirculation:** Controls a 12V pump to move the "wort" through the grain bed to ensure even heating.



### 2. Lautering & Sparging (Rinsing)

**The Goal:** Separate the sugary liquid (wort) from the spent husks and rinse the remaining sugar off the grain.

* **Nano’s Job:** * **Flow Control:** If you use a flow meter, the Nano can calculate exactly how many liters of "sparge water" have been added.
* **Level Sensing:** Using an ultrasonic or float sensor, the Nano can prevent the kettle from overflowing or the pump from running dry.



### 3. The Boil

**The Goal:** Boil the wort for 60–90 minutes to sterilize it, concentrate the sugars, and extract bitterness from hops.

* **Nano’s Job:** * **Vigor Control:** The Nano doesn't need PID here (boiling is always 100°C), but it can control the **Duty Cycle**. For example, it might run the heater at 70% power to maintain a "rolling boil" without it boiling over.
* **Timed Additions:** The Nano counts down and triggers a buzzer when it’s time to add "Bittering," "Flavor," or "Aroma" hops.



### 4. Cooling (Chilling)

**The Goal:** Drop the temperature from 100°C to roughly 20°C as fast as possible to prevent infection and prepare for yeast.

* **Nano’s Job:** * **Efficiency Monitoring:** By checking the temperature of both the beer and the outflowing cooling water, the Nano can calculate the cooling rate and alert you when the "Pitching Temperature" is reached.

### 5. Fermentation

**The Goal:** Yeast eats the sugar and turns it into alcohol and $CO_2$. This takes 1–2 weeks.

* **Nano’s Job:** * **Fermentation Chamber Control:** The Nano can control a fridge (for cooling) and a heat belt (for warming) simultaneously to keep the yeast at a steady temperature (e.g., 18°C).
* **Data Logging:** It can send temperature data over Serial (or to an SD card) so you can see a graph of the fermentation "curve."



---

### Hardware Mapping Summary

| Stage | Nano Component | Action |
| --- | --- | --- |
| **Mash** | DS18B20 + SSR | Maintain exact enzyme activation temp |
| **Boil** | Piezo Buzzer | Alert for hop additions at 60, 30, and 5 mins |
| **Cooling** | Dual Temp Probes | Monitor heat exchange efficiency |
| **Ferment** | Relay Module | Toggle a fridge/heater to keep yeast stable |

---

### Example Logic Flow

In your C++ code, the "Main Loop" would likely look like a series of **Cases**:

1. **Case STRIKE:** Heat water to 72°C. Once reached, beep and move to Case MASH.
2. **Case MASH:** Hold 65°C for 60 minutes. Run pump. When timer = 0, move to Case BOIL.
3. **Case BOIL:** Run heater at 80% power. Start hop timers.

Do you plan on using a physical button to "advance" through these stages, or do you want the Nano to handle the transitions automatically based on the timers?
