CREATE TABLE `water` (
  `idwater` int(11) NOT NULL AUTO_INCREMENT,
  `arduino` varchar(10) NOT NULL,
  `cold_value` varchar(45) NOT NULL,
  `warm_value` varchar(45) NOT NULL,
  `datetime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`idwater`)
) ENGINE=InnoDB AUTO_INCREMENT=33 DEFAULT CHARSET=latin1
