---
title: Protocol - CAN Bus
date: 2021-06-23 12:00:00 -0500
categories: [Documentation, Protocol]
tags: [documentation, protocol, can-bus]
toc: true
---

## CAN ID Format

This CAN ID format uses the 29-bit extended format ID. The first 8 bits serve as message type identifiers, the following 5 bits as message information, and the final 16 bits as sender identification.

All message type bits besides the selected type should be set to `1` with the selected type set to `0`.

All modules use the filter `0x00000000` and mask `0x00010000` to prevent reception of monitor messages, which are low-priority debugging messages with no game impact.

|  Bit  | Name                 | Description                                                          |
| :---: | :------------------- | :------------------------------------------------------------------- |
| `29`  | Timer Update         | Message contains information for universal timer sync.               |
| `28`  | Strike               | Message was triggered by a strike event.                             |
| `27`  | Solve                | Message was triggered by a solve event.                              |
| `26`  | Keep Alive           | Message is a heartbeat message to refresh watchdog.                  |
| `25`  | Setup                | Message relates to module setup process.                             |
| `24`  | End                  | Message relates to the end of the game.                              |
| `23`  | Info                 | Message relates to general information. *(edgework, etc.)*           |
| `22`  | Reserved             | Unused                                                               |
| `21`  | Counter              | 2 bit counter for multi-part messages.                               |
| `20`  | ^^                   | ^^                                                                   |
| `19`  | Is Response          | Message is a response to a previous message. *(1 for response)*      |
| `18`  | Has Target           | Message has a specific intended target. *(1 for targeted)*           |
| `17`  | Monitor              | Message is a log and should be ignored by modules. *(1 for monitor)* |
| `16`  | ID Letter 1          | First letter of Module ID as 5 bit character. *(A=1, B=2, ...)*      |
| `15`  | ^^                   | ^^                                                                   |
| `14`  | ^^                   | ^^                                                                   |
| `13`  | ^^                   | ^^                                                                   |
| `12`  | ^^                   | ^^                                                                   |
| `11`  | ID Letter 2          | Second letter of Module ID as 5 bit character. *(A=1, B=2, ...)*     |
| `10`  | ^^                   | ^^                                                                   |
| `09`  | ^^                   | ^^                                                                   |
| `08`  | ^^                   | ^^                                                                   |
| `07`  | ^^                   | ^^                                                                   |
| `06`  | ID Module Type       | Module type. *(00=Normal, 01=Needy, 10=Custom, 11=Control)*          |
| `05`  | ^^                   | ^^                                                                   |
| `04`  | ID Unique Identifier | 4 bit identifier to uniquely distinguish modules.                    |
| `03`  | ^^                   | ^^                                                                   |
| `02`  | ^^                   | ^^                                                                   |
| `01`  | ^^                   | ^^                                                                   |

## Testing

Testing Testing

Testing
