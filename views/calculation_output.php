
    <h4> Your path will be as follows: </h4>


        <?php foreach($stops as $stop): ?>                         
            <?= $stop ?>
            <br>
        <?php endforeach ?>


    <h4> This trip will cost you  <?= $lowest_cost ?> </h4>
