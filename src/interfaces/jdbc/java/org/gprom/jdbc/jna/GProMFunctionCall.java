package org.gprom.jdbc.jna;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import java.util.Arrays;
import java.util.List;

public class GProMFunctionCall extends GProMStructure {
	public int type;
	/** C type : String */
	public String functionname;
	/** C type : GProMList* */
	public org.gprom.jdbc.jna.GProMList.ByReference args;
	public int isAgg;
	public GProMFunctionCall() {
		super();
	}
	public GProMFunctionCall(Pointer address) {
		super(address);
	}
	protected List<? > getFieldOrder() {
		return Arrays.asList("type", "functionname", "args", "isAgg");
	}
	/**
	 * @param functionname C type : String<br>
	 * @param args C type : GProMList*
	 */
	public GProMFunctionCall(int type, String functionname, org.gprom.jdbc.jna.GProMList.ByReference args, int isAgg) {
		super();
		this.type = type;
		this.functionname = functionname;
		this.args = args;
		this.isAgg = isAgg;
	}
	public static class ByReference extends GProMFunctionCall implements Structure.ByReference {
		
	};
	public static class ByValue extends GProMFunctionCall implements Structure.ByValue {
		
	};
}
