package org.gprom.jdbc.jna;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import java.util.Arrays;
import java.util.List;
public class GProMAggregationOperator extends GProMStructure {
	/** C type : GProMQueryOperator */
	public GProMQueryOperator op;
	/**
	 * aggregation expressions, FunctionCall type<br>
	 * C type : GProMList*
	 */
	public org.gprom.jdbc.jna.GProMList.ByReference aggrs;
	/**
	 * group by expressions<br>
	 * C type : GProMList*
	 */
	public org.gprom.jdbc.jna.GProMList.ByReference groupBy;
	public GProMAggregationOperator() {
		super();
	}
	public GProMAggregationOperator(Pointer address) {
		super(address);
	}
	protected List<? > getFieldOrder() {
		return Arrays.asList("op", "aggrs", "groupBy");
	}
	/**
	 * @param op C type : GProMQueryOperator<br>
	 * @param aggrs aggregation expressions, FunctionCall type<br>
	 * C type : GProMList*<br>
	 * @param groupBy group by expressions<br>
	 * C type : GProMList*
	 */
	public GProMAggregationOperator(GProMQueryOperator op, org.gprom.jdbc.jna.GProMList.ByReference aggrs, org.gprom.jdbc.jna.GProMList.ByReference groupBy) {
		super();
		this.op = op;
		this.aggrs = aggrs;
		this.groupBy = groupBy;
	}
	public static class ByReference extends GProMAggregationOperator implements Structure.ByReference {
		
	};
	public static class ByValue extends GProMAggregationOperator implements Structure.ByValue {
		
	};
}
