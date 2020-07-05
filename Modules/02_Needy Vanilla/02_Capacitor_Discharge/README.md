# Capacitor Discharge
*I'm going to guess that this is just meant to occupy your attention, because otherwise this is some shoddy electronics work.*

## Behavior
This module will activate for the first time when:
- 1:30 has elapsed
- A strike occurs
- A module is solved

The module plays the sound `needy_activated.wav` when activated.

Once activated the Needy Timer will count down from a maximum value of `45`. If left alone this module will "charge up" as the counter decreases, with a bar along the left edge of the module showing the progress. At a countdown value of `25` a sine-wave sound begins at `297.8Hz` and climbs in volume and pitch to `451.2Hz` at a countdown value of `0`, at which point the capacitor explodes.

At this point a strike is awarded and the module is permanently disabled.

When the Discharge Lever is held down the timer will restore at a rate of `5` units per second until the maximum is reached. During this time a light at the right side of the module will light at a brightness inversely proportional to the timer value (off at `45` and fully lit at `0`) and the bar along the left side will fall to reflect the state of the timer. If a sound was playing it will fall back to `297.8Hz` and no volume by the time the counter as reached `25`.

Holding the lever after the timer is fully reset to default will prevent it from progressing. On release the module continues as normal.
