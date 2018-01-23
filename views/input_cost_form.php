
    <h4> You want to go to  <?= htmlspecialchars($_POST["num_cities"]) ?> cities! 
    Provide the cost of going to each city and we'll tell you the best route to take. </h4>
    
    <form action="calculate.php" method="post">
        <table>
            <?php for($i = 1; $i<= $_POST["num_cities"]; $i++): ?>
                <tr>
                    <th> City <?= htmlspecialchars($i) ?> </th>
                    
                    <?php for($j = 1; $j<= $_POST["num_cities"]; $j++): ?>                         
                        <?php if($i == $j): ?>
                            <td> N/A </td>
                        <?php else: ?>   
                            <td> <input autocomplete="off" name="<?= $i ?>_<?= $j ?>" 
                                placeholder="From city <?= $i ?> to city <?= $j ?>" type="text"/></td>
                        <?php endif ?>
                    <?php endfor ?>
                </tr>
    
            <?php endfor ?>
        </table>
    <br>
    <input type="submit" value="Submit!"/>
    </form>
