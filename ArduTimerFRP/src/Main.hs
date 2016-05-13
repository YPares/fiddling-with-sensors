module Main where

import Arduino.Uno

main = compileProgram $ do

    digitalOutput pin13 =: analogRead a0 ~> mapS f

      where f x = if_ (greater x 500) bitLow bitHigh
