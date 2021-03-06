package org.gprom.jdbc.jna;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import java.util.Arrays;
import java.util.List;
public class GProMAttributeReference extends GProMStructure {
	/**
	 * @see GProMNodeTag<br>
	 * C type : GProMNodeTag
	 */
	public int type;
	/** C type : const char* */
	public String name;
	public int fromClauseItem;
	public int attrPosition;
	public int outerLevelsUp;
	/**
	 * @see GProMDataType<br>
	 * C type : GProMDataType
	 */
	public int attrType;
	public GProMAttributeReference() {
		super();
	}
	public GProMAttributeReference(Pointer address) {
		super(address);
	}
	protected List<? > getFieldOrder() {
		return Arrays.asList("type", "name", "fromClauseItem", "attrPosition", "outerLevelsUp", "attrType");
	}
	/**
	 * @param type @see GProMNodeTag<br>
	 * C type : GProMNodeTag<br>
	 * @param name C type : const char*<br>
	 * @param attrType @see GProMDataType<br>
	 * C type : GProMDataType
	 */
	public GProMAttributeReference(int type, String name, int fromClauseItem, int attrPosition, int outerLevelsUp, int attrType) {
		super();
		this.type = type;
		this.name = name;
		this.fromClauseItem = fromClauseItem;
		this.attrPosition = attrPosition;
		this.outerLevelsUp = outerLevelsUp;
		this.attrType = attrType;
	}
	public static class ByReference extends GProMAttributeReference implements Structure.ByReference {
		
	};
	public static class ByValue extends GProMAttributeReference implements Structure.ByValue {
		
	};
}
