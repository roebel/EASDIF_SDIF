
public class main {
    static {
	System.loadLibrary("eaSDIF");
	eaSDIF.EasdifInit("");
    }
    
    public static void main(String argv[]) {
	boolean res;
	Entity file = new Entity();
	System.out.println("created new SDIFEntity");
	Frame frame = new Frame();
	System.out.println("created new SDIFFrame");
	res = file.OpenRead("../../test/lic.sdif");
	System.out.println("open (mode " + SdifFileModeET.eReadFile + ")..." + res);
	while (!file.eof())
	    {
		int ress = file.ReadNextFrame(frame);
		String fsig = frame.GetSignature();
		Matrix mat = frame.GetMatrix(0);
		String msig = mat.GetSignature();
		int nrow = mat.GetNbRows();
		int ncol = mat.GetNbCols();
		double val = mat.GetDouble(0, 0);
		System.out.println("frame " + fsig + " matrix " + msig + "(" + nrow + "," + ncol + ") = " + val);
	    }
	file.Close();
    }
}
