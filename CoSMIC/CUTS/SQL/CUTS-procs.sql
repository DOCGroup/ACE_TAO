-- $Id$ --

DELIMITER //

use cuts

--
-- This process retruns all the components in a given
-- path.
--
-- COLUMNS: [path_order]
--          [instance]
--          [src]
--          [dst]
--
-- ORDER: [path_order]
--

DROP PROCEDURE IF EXISTS select_path;

CREATE
  PROCEDURE select_path (IN path INT)
BEGIN
  SELECT path_order, instance, src, dst
    FROM critical_path_elements
    WHERE path_id = path
    ORDER BY path_order;
END;

--
-- This procedure returns all the metrics for components in a
-- specified path.
--
-- COLUMNS: [collection_time]
--          [component]
--          [sender]
--          [src]
--          [dst]
--          [metric_type]
--          [metric_count]
--          [best_time]
--          [average_time]
--          [worse_time]
--
-- ORDER: [collection_time][component][sender]
--

DROP PROCEDURE IF EXISTS select_path_execution_times;

CREATE
  PROCEDURE select_path_execution_times (IN test INT, IN path INT)
BEGIN
  SELECT collection_time, component, sender, cp.src, cp.dst, metric_type,
    metric_count, best_time, average_time, worse_time
    FROM critical_path_elements AS cp
    LEFT JOIN execution_time AS et ON (
      cp.instance = et.component AND cp.src = et.src AND (cp.dst = et.dst OR
      et.dst IS NULL))
    WHERE (path_id = path AND test_number = test)
    ORDER BY collection_time, component, sender;
END;

//

DELIMITER ;
