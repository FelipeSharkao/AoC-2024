main :: IO ()
main = do
    exampleStr <- readFile "example.txt"
    let example = map (map read) $ map words $ lines exampleStr
    _ <- putStrLn $ "Example res: " ++ (show $ solve $ example)

    inputStr <- readFile "input.txt"
    let input = map (map read) $ map words $ lines inputStr
    putStrLn $ "Res: " ++ (show $ solve $ input)

solve reports = sum $ map fromEnum $ map isSafeLevel reports

isSafeLevel report = case report of
    a:b:rest  -> isSafeLevelP report $ if a < b then (<) else (>)
    otherwise -> True

isSafeLevelP report p = case report of
    a:b:rest ->
        let d = abs (a-b)
        in if (p a b) && d >= 1 && d <= 3
            then isSafeLevelP (b:rest) p
            else False
    otherwise -> True

-- dbg :: Show a => String -> a -> a
-- dbg prefix x = trace (prefix ++ ": " ++ (show x)) x
