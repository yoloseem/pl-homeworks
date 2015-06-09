iSqrt :: Int -> Int
iSqrt = floor.sqrt.fromIntegral

isPrime :: Int -> Bool
isPrime 1 = False
isPrime x = not(any(\y -> x `mod` y == 0) [2..iSqrt(x)])

findingPrimes :: Int -> Int -> [Int]
findingPrimes a b = take b (filter isPrime [a..])
