-- written by Abhay Prakash
SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- Database: `userlog`
--
CREATE DATABASE IF NOT EXISTS `userlog` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci;
USE `userlog`;

-- --------------------------------------------------------

--
-- Table structure for table `session`
--

CREATE TABLE IF NOT EXISTS sessions (
  `SessionID` int(11) NOT NULL,
  `USERID` int(11) NOT NULL,
  `SessionDay` tinyint(4) NOT NULL,
  PRIMARY KEY (`SessionID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE INDEX index_session_userID ON sessions(USERID);

-- --------------------------------------------------------

--
-- Table structure for table `train_query`
--

CREATE TABLE IF NOT EXISTS `train_query` (
  `SessionID` int(11) NOT NULL,
  `QueryID` int(11) NOT NULL,
  PRIMARY KEY (`QueryID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE INDEX index_trainq_SessionID ON train_query(SessionID);

-- --------------------------------------------------------

--
-- Table structure for table `query`
--

CREATE TABLE IF NOT EXISTS `query` (
  `QueryID` int(11) NOT NULL,
  `SessionID` int(11) NOT NULL,
  `QueryMadeAtTime` int(11) NOT NULL,
  ClickedAnyLink tinyint( 1 ) DEFAULT  0,
  TimeForFirstClick int(11) DEFAULT NULL,
  PRIMARY KEY (`QueryID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE INDEX index_query_SessionID ON query(SessionID);
CREATE INDEX index_query_queryTime ON query(QueryMadeAtTime);

-- --------------------------------------------------------

--
-- Table structure for table `queryshowedlinks`
--

CREATE TABLE IF NOT EXISTS `queryshowedlinks` (
  `SessionID` int(11) NOT NULL,
  `QueryID` int(11) NOT NULL,
  `URLID` int(11) NOT NULL,
  `SERPID` int(11) NOT NULL,
  `DisplayRank` int(11),
  WasClicked tinyint(1) DEFAULT 0,
  `TimeSpent` int(11) DEFAULT NULL,
  `Grade` tinyint(4) DEFAULT 0,
  CONSTRAINT pk_queryshowedlinks PRIMARY KEY (SessionID, QueryID, URLID, SERPID)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE INDEX index_queryshowedlinks_SessionID_QueryID ON queryshowedlinks(SessionID, QueryID);
CREATE INDEX index_queryshowedlinks_QueryID ON queryshowedlinks(QueryID);
CREATE INDEX index_queryshowedlinks_URLID ON queryshowedlinks(URLID);

-- --------------------------------------------------------

--
-- Table structure for table `url`
--

CREATE TABLE IF NOT EXISTS `url` (
  `URLID` int(11) NOT NULL,
  `DomainID` int(11) NOT NULL,
  PRIMARY KEY (`URLID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `queryhasterms`
--

CREATE TABLE IF NOT EXISTS `queryhasterms` (
  `QueryID` int(11) NOT NULL,
  `TermID` int(11) NOT NULL,
  PRIMARY KEY (`QueryID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `test_query`
--

CREATE TABLE IF NOT EXISTS `test_query` (
  `SessionID` int(11) NOT NULL,
  `QueryID` int(11) NOT NULL,
  PRIMARY KEY (`QueryID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE INDEX index_testq_SessionID ON test_query(SessionID);

-- --------------------------------------------------------

--
-- Table structure for table `validate_query`
--

CREATE TABLE IF NOT EXISTS `validate_query` (
  `SessionID` int(11) NOT NULL,
  `QueryID` int(11) NOT NULL,
  PRIMARY KEY (`QueryID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE INDEX index_validateq_SessionID ON validate_query(SessionID);
