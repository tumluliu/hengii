-- MySQL dump 10.13  Distrib 5.1.52, for unknown-linux-gnu (x86_64)
--
-- Host: 127.0.0.1    Database: higis
-- ------------------------------------------------------
-- Server version	5.1.52

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `gdos_sys_data`
--

DROP TABLE IF EXISTS `gdos_sys_data`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gdos_sys_data` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `datausage` int(11) DEFAULT '1',
  `name` varchar(255) DEFAULT NULL,
  `flowid` bigint(20) NOT NULL,
  `datatype` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_Reference_6` (`flowid`),
  CONSTRAINT `FK_Reference_6` FOREIGN KEY (`flowid`) REFERENCES `gdos_sys_jobflow` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=44 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gdos_sys_data_log`
--

DROP TABLE IF EXISTS `gdos_sys_data_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gdos_sys_data_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `jobid` int(11) DEFAULT NULL,
  `user` varchar(256) DEFAULT NULL,
  `starttime` datetime DEFAULT NULL,
  `endtime` datetime DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `content` varchar(2048) DEFAULT NULL,
  `data_id` bigint(20) DEFAULT NULL,
  `name` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gdos_sys_job`
--

DROP TABLE IF EXISTS `gdos_sys_job`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gdos_sys_job` (
  `id` int(11) NOT NULL,
  `fid` bigint(20) NOT NULL,
  `status` int(11) DEFAULT NULL,
  `message` text,
  `inpath` varchar(200) DEFAULT NULL,
  `outpath` varchar(200) DEFAULT NULL,
  `intype` int(11) DEFAULT NULL,
  `outype` int(11) DEFAULT NULL,
  `ispbsjob` int(11) DEFAULT NULL,
  `appuri` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`,`fid`),
  KEY `FK_Reference_5` (`fid`),
  CONSTRAINT `FK_Reference_5` FOREIGN KEY (`fid`) REFERENCES `gdos_sys_jobflow` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gdos_sys_jobflow`
--

DROP TABLE IF EXISTS `gdos_sys_jobflow`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gdos_sys_jobflow` (
  `id` bigint(20) NOT NULL,
  `status` int(11) DEFAULT NULL,
  `ctime` datetime DEFAULT NULL,
  `user` varchar(256) DEFAULT NULL,
  `utime` datetime DEFAULT NULL,
  `message` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gdos_sys_server`
--

DROP TABLE IF EXISTS `gdos_sys_server`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gdos_sys_server` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(50) DEFAULT NULL,
  `post` varchar(50) DEFAULT NULL,
  `name` varchar(50) DEFAULT NULL,
  `status` int(11) DEFAULT NULL,
  `username` varchar(50) DEFAULT NULL,
  `userpwd` varchar(50) DEFAULT NULL,
  `apppath` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gdos_sys_source`
--

DROP TABLE IF EXISTS `gdos_sys_source`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gdos_sys_source` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(255) DEFAULT NULL,
  `dataschema` varchar(255) DEFAULT NULL,
  `tempschema` varchar(255) DEFAULT NULL,
  `username` varchar(255) DEFAULT NULL,
  `userpwd` varchar(255) DEFAULT NULL,
  `port` int(11) DEFAULT NULL,
  `dtype` int(11) DEFAULT NULL,
  `seq` int(11) DEFAULT NULL,
  `nodetype` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gdos_sys_tools`
--

DROP TABLE IF EXISTS `gdos_sys_tools`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gdos_sys_tools` (
  `app_id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) DEFAULT NULL,
  `created_at` datetime DEFAULT NULL,
  `updated_at` datetime DEFAULT NULL,
  `type` char(50) DEFAULT NULL,
  `abstract` text,
  `language` varchar(50) DEFAULT NULL,
  `version` varchar(50) DEFAULT NULL,
  `author` varchar(50) DEFAULT NULL,
  `uri` varchar(200) DEFAULT NULL,
  `org` varchar(50) DEFAULT NULL,
  `parallel_programming_model` varchar(200) DEFAULT NULL,
  `parallel_performance_overview` varchar(200) DEFAULT NULL,
  `category` varchar(20) DEFAULT NULL,
  `path` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`app_id`)
) ENGINE=InnoDB AUTO_INCREMENT=108 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gdos_sys_tools_options`
--

DROP TABLE IF EXISTS `gdos_sys_tools_options`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gdos_sys_tools_options` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `app_id` int(11) NOT NULL,
  `name` varchar(50) DEFAULT NULL,
  `description` text,
  `created_at` datetime DEFAULT NULL,
  `updated_at` datetime DEFAULT NULL,
  `value` varchar(50) DEFAULT NULL,
  `type` varchar(50) DEFAULT NULL,
  `valuexml` text,
  `seq` varchar(50) DEFAULT NULL,
  `style` varchar(200) DEFAULT NULL,
  `showname` varchar(200) DEFAULT NULL,
  `isnull` int(11) DEFAULT NULL,
  `switch` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_Reference_3` (`app_id`),
  CONSTRAINT `FK_Reference_3` FOREIGN KEY (`app_id`) REFERENCES `gdos_sys_tools` (`app_id`)
) ENGINE=InnoDB AUTO_INCREMENT=652 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gdos_sys_tools_users`
--

DROP TABLE IF EXISTS `gdos_sys_tools_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gdos_sys_tools_users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `email` varchar(20) NOT NULL,
  `app_id` int(11) NOT NULL,
  `createtime` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_Reference_1` (`email`),
  KEY `FK_Reference_2` (`app_id`),
  CONSTRAINT `FK_Reference_1` FOREIGN KEY (`email`) REFERENCES `gdos_sys_users` (`email`),
  CONSTRAINT `FK_Reference_2` FOREIGN KEY (`app_id`) REFERENCES `gdos_sys_tools` (`app_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `gdos_sys_users`
--

DROP TABLE IF EXISTS `gdos_sys_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gdos_sys_users` (
  `email` varchar(20) NOT NULL,
  `userpwd` varchar(50) DEFAULT NULL,
  `name` varchar(50) DEFAULT NULL,
  `sex` int(11) DEFAULT NULL,
  `birthdate` datetime DEFAULT NULL,
  `phone` varchar(50) DEFAULT NULL,
  `tdesc` varchar(200) DEFAULT NULL,
  `createtime` datetime DEFAULT NULL,
  `lasttime` datetime DEFAULT NULL,
  `isadmin` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`email`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-05-22 17:57:49
-- MySQL dump 10.13  Distrib 5.1.52, for unknown-linux-gnu (x86_64)
--
-- Host: 127.0.0.1    Database: higis
-- ------------------------------------------------------
-- Server version	5.1.52

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `GDOS_SYS_FEATURES`
--

DROP TABLE IF EXISTS `GDOS_SYS_FEATURES`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GDOS_SYS_FEATURES` (
  `data_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `owner` varchar(256) DEFAULT NULL,
  `name` varchar(256) DEFAULT NULL,
  `table_name` varchar(256) DEFAULT NULL,
  `create_date` datetime DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `geometry_type` int(11) DEFAULT NULL,
  `geometry_column` varchar(256) DEFAULT NULL,
  `srid` int(11) DEFAULT NULL,
  `column_count` int(11) DEFAULT NULL,
  `feature_count` int(11) DEFAULT NULL,
  `domain_minx` double DEFAULT NULL,
  `domain_miny` double DEFAULT NULL,
  `domain_maxx` double DEFAULT NULL,
  `domain_maxy` double DEFAULT NULL,
  `srtext` varchar(2048) DEFAULT NULL,
  `description` varchar(2048) DEFAULT NULL,
  PRIMARY KEY (`data_id`)
) ENGINE=InnoDB AUTO_INCREMENT=39 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `GDOS_SYS_RASTERS`
--

DROP TABLE IF EXISTS `GDOS_SYS_RASTERS`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GDOS_SYS_RASTERS` (
  `data_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `owner` varchar(256) DEFAULT NULL,
  `name` varchar(256) DEFAULT NULL,
  `source_name` varchar(256) DEFAULT NULL,
  `gdal_driver` varchar(256) DEFAULT NULL,
  `create_date` datetime DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `subdataset` int(11) DEFAULT NULL,
  `band_count` int(11) DEFAULT NULL,
  `pixel_type` int(11) DEFAULT NULL,
  `pixel_depth` int(11) DEFAULT NULL,
  `pyramid_level` int(11) DEFAULT NULL,
  `x_size` int(11) DEFAULT NULL,
  `y_size` int(11) DEFAULT NULL,
  `resolution_x` double DEFAULT NULL,
  `resolution_y` double DEFAULT NULL,
  `srid` int(11) DEFAULT NULL,
  `domain_minx` double DEFAULT NULL,
  `domain_miny` double DEFAULT NULL,
  `domain_maxx` double DEFAULT NULL,
  `domain_maxy` double DEFAULT NULL,
  `srtext` varchar(2048) DEFAULT NULL,
  `description` varchar(2048) DEFAULT NULL,
  PRIMARY KEY (`data_id`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-05-22 17:57:49
