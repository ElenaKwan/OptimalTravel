<?php

    // configuration
    require("../includes/config.php"); 
    
    // if user reached page via GET (as by clicking a link, via redirect or by refreshing the page)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        redirect("index.php");
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
    
        // iterate through the inputs and store as a single string
        $data = "";
        
        for($i = 1; $i<= $_SESSION["num_cities"]; $i++)
        {
            for($j = 1; $j<= $_SESSION["num_cities"]; $j++)
            {
                if($i == $j)
                {
                    $data = $data. $i . ' ' . $j . ' ' . 2147483647 . "\n";
                }
                else
                {
                    // check to ensure that the input is a number between 0 and 2147483646 (2147483647 is reserved for "infinity" as a way to negate entries)
                    if(is_numeric($_POST["{$i}_{$j}"]) && $_POST["{$i}_{$j}"] >= 0 && $_POST["{$i}_{$j}"] < 2147483647)
                    {
                        $data = $data. $i . ' ' . $j . ' ' . $_POST["{$i}_{$j}"]. "\n";
                    }
                    else 
                    {
                       apologize("You must provide a number between (and inclusive of) 0 - 2147483646 for all items");
                    }
                }
            }
        }
        
        // store the string into a file
        $ret = file_put_contents('mydata.txt', $data, LOCK_EX);
        if($ret === false) 
        {
            die('There was an error writing this file');
        }
        
        // execute the travelling salesman branch-and-bound program written in c (TSP.c)
        $output = shell_exec("./TSP {$_SESSION['num_cities']} mydata.txt 2>&1");
        

        // parse individual words from $outputs and store in an array.
        $output = trim($output);
        $output_arr = preg_split('/\s+/', $output);
        
        // find the index of the lowest cost and path outputs. 
        // Since the output of TSP.c is formatted as: "the lower bound is : x optimal path is : city1 city2 city3 ...",
        // the values of interest come after the a colon (:).  
        $letter=':';
        $letterPositions = array_keys($output_arr,$letter);
        
        // index of lowest_cost and path in the $output_arr
        $lowest_cost_index = $letterPositions[0] + 1; 
        $path_index = $letterPositions[1] + 1; 
        
        // save the outputs to an array which will be passed pass to the calculation_output page to be rendered
        $lowest_cost = $output_arr[$lowest_cost_index];
        $stops;
        $count = 0;
        
        for($k = $path_index; $k < count($output_arr); $k++)
        {
            $stops[$count] = $output_arr[$k];
            $count++;
        }
        
        // render the output page
        render("calculation_output.php", ["lowest_cost" => $lowest_cost, "stops" => $stops]);

    }
    

?>