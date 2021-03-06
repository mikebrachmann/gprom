/**
 * 
 */
package org.gprom.jdbc.jna;

import java.util.Arrays;
import java.util.List;

import com.sun.jna.Callback;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.ptr.PointerByReference;
public class GProMMetadataLookupPlugin extends Structure {
	/** C type : isInitialized_callback* */
	public GProMMetadataLookupPlugin.isInitialized_callback isInitialized;
	/** C type : initMetadataLookupPlugin_callback* */
	public GProMMetadataLookupPlugin.initMetadataLookupPlugin_callback initMetadataLookupPlugin;
	/** C type : databaseConnectionOpen_callback* */
	public GProMMetadataLookupPlugin.databaseConnectionOpen_callback databaseConnectionOpen;
	/** C type : databaseConnectionClose_callback* */
	public GProMMetadataLookupPlugin.databaseConnectionClose_callback databaseConnectionClose;
	/** C type : shutdownMetadataLookupPlugin_callback* */
	public GProMMetadataLookupPlugin.shutdownMetadataLookupPlugin_callback shutdownMetadataLookupPlugin;
	/** C type : catalogTableExists_callback* */
	public GProMMetadataLookupPlugin.catalogTableExists_callback catalogTableExists;
	/** C type : catalogViewExists_callback* */
	public GProMMetadataLookupPlugin.catalogViewExists_callback catalogViewExists;
	/** C type : getKeyInformation_callback* */
	public GProMMetadataLookupPlugin.getKeyInformation_callback getKeyInformation;
	/** C type : getAttributes_callback* */
	public GProMMetadataLookupPlugin.getDataTypes_callback getDataTypes;
	/** C type : getAttributeNames_callback* */
	public GProMMetadataLookupPlugin.getAttributeNames_callback getAttributeNames;
	/** C type : getAttributeDefaultVal_callback* */
	public GProMMetadataLookupPlugin.getAttributeDefaultVal_callback getAttributeDefaultVal;
	/** C type : isAgg_callback* */
	public GProMMetadataLookupPlugin.isAgg_callback isAgg;
	/** C type : isWindowFunction_callback* */
	public GProMMetadataLookupPlugin.isWindowFunction_callback isWindowFunction;
	/** C type : getFuncReturnType_callback* */
	public GProMMetadataLookupPlugin.getFuncReturnType_callback getFuncReturnType;
	/** C type : getOpReturnType_callback* */
	public GProMMetadataLookupPlugin.getOpReturnType_callback getOpReturnType;
	/** C type : getTableDefinition_callback* */
	public GProMMetadataLookupPlugin.getTableDefinition_callback getTableDefinition;
	/** C type : getViewDefinition_callback* */
	public GProMMetadataLookupPlugin.getViewDefinition_callback getViewDefinition;
	public interface isInitialized_callback extends Callback {
		int apply();
	};
	public interface initMetadataLookupPlugin_callback extends Callback {
		int apply();
	};
	public interface databaseConnectionOpen_callback extends Callback {
		int apply();
	};
	public interface databaseConnectionClose_callback extends Callback {
		int apply();
	};
	public interface shutdownMetadataLookupPlugin_callback extends Callback {
		int apply();
	};
	public interface catalogTableExists_callback extends Callback {
		int apply(String tableName);
	};
	public interface catalogViewExists_callback extends Callback {
		int apply(String viewName);
	};
	public interface getKeyInformation_callback extends Callback {
		String apply(String tableName);
	};
	public interface getDataTypes_callback extends Callback {
		String apply(String tableName);
	};
	public interface getAttributeNames_callback extends Callback {
		String apply(String tableName);
	};
	public interface getAttributeDefaultVal_callback extends Callback {
		String apply(String schema, String tableName, String attrName);
	};
	public interface isAgg_callback extends Callback {
		int apply(String functionName);
	};
	public interface isWindowFunction_callback extends Callback {
		int apply(String functionName);
	};
	public interface getFuncReturnType_callback extends Callback {
		String apply(String fName, PointerByReference args, int numArgs);
	};
	public interface getOpReturnType_callback extends Callback {
		String apply(String oName, PointerByReference args, int numArgs);
	};
	public interface getTableDefinition_callback extends Callback {
		String apply(String tableName);
	};
	public interface getViewDefinition_callback extends Callback {
		String apply(String viewName);
	};
	public GProMMetadataLookupPlugin() {
		super();
	}
	protected List<? > getFieldOrder() {
		return Arrays.asList("isInitialized", "initMetadataLookupPlugin", "databaseConnectionOpen", "databaseConnectionClose", "shutdownMetadataLookupPlugin", "catalogTableExists", "catalogViewExists", "getKeyInformation", "getDataTypes", "getAttributeNames", "getAttributeDefaultVal", "isAgg", "isWindowFunction", "getFuncReturnType", "getOpReturnType", "getTableDefinition", "getViewDefinition");
	}
	public GProMMetadataLookupPlugin(Pointer peer) {
		super(peer);
	}
	public static class ByReference extends GProMMetadataLookupPlugin implements Structure.ByReference {
		
	};
	public static class ByValue extends GProMMetadataLookupPlugin implements Structure.ByValue {
		
	};
}

