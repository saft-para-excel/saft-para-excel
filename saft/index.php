<?php
    include_once 'database.php';
    ini_set('display_errors', '1');
    ini_set('display_startup_errors', '1');
    error_reporting(E_ALL);
?>
<html lang="en">
<link rel="stylesheet" href="css/style_saft.css">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Programas Certificados</title>
    </head>
<body>
<script src="https://code.jquery.com/jquery-3.3.1.js" ></script>
<script>
function showHint(str) {
    var request;
    if (str.length == 0) {
        document.getElementById("txtHint").innerHTML = "";
        return;
    } else {
        request = $.ajax({
        url: "gethint.php?q=" + str,
        type: "get",
        data: str,success: function (response) {//once the request successfully process to the server side it will return result here
                //parse json
                // response = JSON.parse(response);
               console.log(response)
                // document.getElementById('dxtable').innerHTML = response[0].toString();
                document.getElementById('dxtable').innerHTML = response;
            }
                });
            }
    }

</script>
<?php $sql='SELECT * from programas order by certificate_num desc;'; ?>
<form>
<div class="lin">
<div class="crit" > <label>Pesquisa </label>
    <input class="texte-filtre" type="text"  id="txtName" name="search" onkeyup="showHint(this.value)" >
    <button type="button" name="call-search" ">Click</button>
</div>
</div>
</form>
 <p>Matches: <span id="txtHint"></span></p>
<table class="zui-table" id="dxtable">
<tr  >
<th>Num</th>
<th>Programa</th>
<th>Versão</th>
<th>Editor</th>
<th>Data</th>
</tr>
<?php foreach (query_output_many($sql) as $row): ?>
<tr>
    <td><a href="cert_edit.php?id=<?php echo $row['certificate_num'] ?>"  target="_blank" > <?php echo $row['certificate_num'] ?></a></td>
    <td> <?php echo $row['name'] ?></a></td>
<td ><?= htmlspecialchars($row['version']) ?></td>
<td><?= htmlspecialchars($row['producer']) ?></td>
<td> <?= htmlspecialchars($row['date']) ?></td>

    </tr>
<?php endforeach; ?>
</table>
</body>
</html>
