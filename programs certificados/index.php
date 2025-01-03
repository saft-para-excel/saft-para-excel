<?php
    include_once 'database.php';
?>
<html lang="en">
<link rel="stylesheet" href="css/style.css">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>Programas Certificados pela AT</title>
    </head>
<body>
<script src="https://code.jquery.com/jquery-3.3.1.js" ></script>

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
