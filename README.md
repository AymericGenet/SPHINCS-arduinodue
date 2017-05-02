Introduction
============

This project implements the stateless hash-based signature scheme SPHINCS-256 for an [Arduino Due board](https://www.arduino.cc/en/Main/arduinoBoardDue). The scheme provides an effective replacement for digital signature in a post-quantum world. It combines the extended Merkle's signature scheme (XMSS), improved Winternitz one-time signatures (WOTS+), and "Hash to Obtain Random Subset" Trees (HORST) few-time signatures scheme.

The goal of this project is to to reproduce the implementation of [SPHINCS-256 on an STM32L100XC ARM Cortex-M3](https://joostrijneveld.nl/papers/armedsphincs/) but for a SAM3X8E ARM Cortex-M3 CPU using the Atmel Studio framework (ASF) such that environmental attacks, like side-channel analysis or fault injection, can be performed. 

SPHINCS
=======

See the official website: https://sphincs.cr.yp.to/.

Requirements
============

The project was intended to work with the following software :

 * An Atmel SAM3X8E ARM Cortex-M3 board ([Arduino Due](https://www.arduino.cc/en/Main/arduinoBoardDue))
 * [Arduino IDE 1.8.1](https://www.arduino.cc/en/Main/Software)
 * [Atmel Studio 7.0](http://www.atmel.com/microsite/atmel-studio/)
 * [Program Arduino Due with Atmel Studio 7](http://www.elecrom.com/program-arduino-due-atmel-studio/)

References
==========

 * Daniel J. Bernstein et al. "SPHINCS: practical stateless hash-based signatures". In: *IACR Cryptology ePrint Archive* 2014 (2014), p. 795. url: http://eprint.iacr.org/2014/795.
 * Andreas Hülsing, Joost Rijneveld, and Peter Schwabe. "ARMed SPHINCS - Computing a 41KB signature in 16KB of RAM". In: *IACR Cryptology ePrint Archive* 2015 (2015), p. 1042. url: http://eprint.iacr.org/2015/1042.

Contact
=======

Author : Aymeric Genet `aymeric {dot} genet {at} epfl {dot} ch`