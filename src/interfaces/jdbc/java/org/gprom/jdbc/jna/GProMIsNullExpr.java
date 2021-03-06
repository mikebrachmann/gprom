package org.gprom.jdbc.jna;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import java.util.Arrays;
import java.util.List;

public class GProMIsNullExpr extends GProMStructure {
	public int type;
	/** C type : GProMNode* */
	public org.gprom.jdbc.jna.GProMNode.ByReference expr;
	public GProMIsNullExpr() {
		super();
	}
	public GProMIsNullExpr(Pointer address) {
		super(address);
	}
	protected List<? > getFieldOrder() {
		return Arrays.asList("type", "expr");
	}
	/** @param expr C type : GProMNode* */
	public GProMIsNullExpr(int type, org.gprom.jdbc.jna.GProMNode.ByReference expr) {
		super();
		this.type = type;
		this.expr = expr;
	}
	public static class ByReference extends GProMIsNullExpr implements Structure.ByReference {
		
	};
	public static class ByValue extends GProMIsNullExpr implements Structure.ByValue {
		
	};
}
