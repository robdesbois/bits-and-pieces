#!/usr/bin/runhaskell

import System.Environment
import System.Process
import Text.Regex.Posix

-- Get list of branches from a given repository base
getBranches :: String -> IO [String]
getBranches repo = do
    branches <- readCreateProcess (shell $ "svn ls " ++ repo ++ "/branches/") ""
    return $ lines branches

-- Get last log message for the given (full) path
getLastLog :: String -> IO String
getLastLog path = readCreateProcess (shell $ "svn log -l1 " ++ path) ""

showLogHeader :: String
showLogHeader = "Branch,Author,Date"

-- Process SVN branch & log message to show required details
showLog :: String -> String -> String
showLog branch log = branch ++ "," ++ author ++ "," ++ date
    where (_,_,_,[author,date]) = infoLine =~ "^r[0-9]+ \\| ([^ ]+) \\| ([0-9-]+)" :: (String,String,String,[String])
          infoLine              = lines log !! 1

-- Return Just nth element of array if exists, otherwise Nothing
maybeNthElement :: [a] -> Int -> Maybe a
maybeNthElement xs n
    | n >= length xs = Nothing
    | otherwise      = Just (xs !! n)

-- Get the repository to use
getRepository :: IO (Maybe String)
getRepository = do
    args <- getArgs
    return $ maybeNthElement args 0

-- Show information for all branches in given repo
showBranchInfo :: String -> IO ()
showBranchInfo repo = do
    branches <- getBranches repo

    let qualBranches = map ((repo ++ "/branches/") ++) branches
    logs <- mapM getLastLog qualBranches

    mapM_ putStrLn $
        showLogHeader : zipWith showLog branches logs

main :: IO ()
main = do
    maybeRepo <- getRepository
    case maybeRepo of
        Nothing   -> error "No repository passed"
        Just repo -> showBranchInfo repo