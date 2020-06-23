
#include "MotatePins.h"

using namespace Motate;

template<> _pinChangeInterrupt * PortHardware<'A'>::_firstInterrupt = nullptr;
template<> _pinChangeInterrupt * PortHardware<'B'>::_firstInterrupt = nullptr;
template<> _pinChangeInterrupt * PortHardware<'C'>::_firstInterrupt = nullptr;
template<> _pinChangeInterrupt * PortHardware<'D'>::_firstInterrupt = nullptr;
