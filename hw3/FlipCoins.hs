flipHT :: String -> Integer -> String
flipHT [] a = []
flipHT xs 0 = xs
flipHT (x:xs) a = [if x == 'H' then 'T' else 'H'] ++
                  (if a > 0 then flipHT xs (a - 1) else xs)


flipCoin' :: String -> Integer -> [Integer]
flipCoin' [] o = [0]
flipCoin' [x] o
    | x == 'H' = [0]
    | otherwise = [o + 1, 0]
flipCoin' (x:xs) o
    | x == y = flipCoin' xs (o + 1)
    | otherwise = [o + 1] ++ flipCoin' xs (o + 1)
    where y = head xs


flipCoin :: String -> [Integer]
flipCoin xs = flipCoin' xs 0
