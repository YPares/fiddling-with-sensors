module Lib where

import Prelude ( putStrLn, IO, Bool )
import Language.Copilot
import Copilot.Compile.C99

import Data.Monoid


{-clockTest :: Spec
clockTest = do
  observer "clk" clkStream
  observer "clk1" clk1Stream


clockExamples :: IO ()
clockExamples = do
  prettyPrint clockTest
  putStrLn ""
  putStrLn ""
  interpret 10 clockTest

sumExterns :: Stream Word64
sumExterns = let ex1 = extern "e1" (Just [0..])  -- To simulate with 'interpret'
                 ex2 = extern "e2" Nothing
             in ex1 + ex2
-}

intensity :: Stream Int32
intensity = (255 * (ctrl `mod` 100)) `div` 100
  where ctrl = [0] ++ (1 + ctrl)  -- infinite stream

clkStream :: Stream Bool
clkStream  = clk  ( period 1 ) ( phase 0 )

driveLeds :: Spec
driveLeds = do observer "intensity" intensity
               trigger "write_comps" clkStream
                 [arg (0 :: Stream Int32), arg intensity, arg intensity]

genCFiles = reify driveLeds >>= compile defaultParams
