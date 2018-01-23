<?php

    // configuration
    require("../includes/config.php"); 

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("input_form.php", ["title" => "Input Cost"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // validate submission
        if (empty($_POST["num_cities"])) 
        {
            apologize("You must provide the number of cities");
        }
        if (!is_numeric($_POST["num_cities"]) || ($_POST["num_cities"] < 2))
        {
            apologize("You must provide a positive number, greater than 1");
        }
        if ($_POST["num_cities"] > 25)
        {
            apologize("The algorithm is only able to handle a maximum of 25 cities within a reasonable time span.");
        }
        
        // store the number of cities
        $_SESSION["num_cities"] = $_POST["num_cities"];
        
        // render another form that prompts the user to provide the travel costs of travelling to/from each city
        render("input_cost_form.php", ["num_cities" => $_POST["num_cities"]]);
    }

?>



