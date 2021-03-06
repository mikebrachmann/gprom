package org.gprom.jdbc.jna;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import java.util.Arrays;
import java.util.List;

public class GProMWindowDef extends GProMStructure {
	public int type;
	/** C type : GProMList* */
	public org.gprom.jdbc.jna.GProMList.ByReference partitionBy;
	/** C type : GProMList* */
	public org.gprom.jdbc.jna.GProMList.ByReference orderBy;
	/** C type : GProMWindowFrame* */
	public org.gprom.jdbc.jna.GProMWindowFrame.ByReference frame;
	public GProMWindowDef() {
		super();
	}
	public GProMWindowDef(Pointer address) {
		super(address);
	}
	protected List<? > getFieldOrder() {
		return Arrays.asList("type", "partitionBy", "orderBy", "frame");
	}
	/**
	 * @param partitionBy C type : GProMList*<br>
	 * @param orderBy C type : GProMList*<br>
	 * @param frame C type : GProMWindowFrame*
	 */
	public GProMWindowDef(int type, org.gprom.jdbc.jna.GProMList.ByReference partitionBy, org.gprom.jdbc.jna.GProMList.ByReference orderBy, org.gprom.jdbc.jna.GProMWindowFrame.ByReference frame) {
		super();
		this.type = type;
		this.partitionBy = partitionBy;
		this.orderBy = orderBy;
		this.frame = frame;
	}
	public static class ByReference extends GProMWindowDef implements Structure.ByReference {
		
	};
	public static class ByValue extends GProMWindowDef implements Structure.ByValue {
		
	};
}
