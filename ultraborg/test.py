# Setup the library ready for use
import UltraBorg
# Board #1, address 10
UB1 = UltraBorg.UltraBorg()
UB1.i2cAddress = 10
UB1.Init()
# Board #2, address 11
UB2 = UltraBorg.UltraBorg()
UB2.i2cAddress = 11
UB2.Init()
# Board #3, address 12
UB3 = UltraBorg.UltraBorg()
UB3.i2cAddress = 12
UB3.Init()
