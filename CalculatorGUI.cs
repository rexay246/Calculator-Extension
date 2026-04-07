using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace CalculatorOOP
{
    internal static class Native
    {
        [DllImport("ArithmeticAPI.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr CreateCalculator();

        [DllImport("ArithmeticAPI.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyCalculator(IntPtr handle);

        // enum OperationKind : int { Add=0, Sub=1, Mul=2, Div=3 }
        [DllImport("ArithmeticAPI.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetOperation(IntPtr handle, int kind);

        [DllImport("ArithmeticAPI.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetOperation(IntPtr handle);

        [DllImport("ArithmeticAPI.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void SetName(IntPtr handle, string nameUtf8);

        [DllImport("ArithmeticAPI.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetName(IntPtr handle, StringBuilder outBuf, int bufSize);

        [DllImport("ArithmeticAPI.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern double Compute(IntPtr handle, double a, double b);

        // Unit conversion
        // Hint: It should return double, take (double value, int fromUnit, int toUnit),
        // and use [DllImport("ArithmeticAPI.dll", CallingConvention = CallingConvention.Cdecl)]
        // Mapping must match C++: 0 = Miles, 1 = Feet, 2 = Centimeters
        [DllImport("ArithmeticAPI.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern double ConvertUnits(double value, int fromUnit, int toUnit);
    }

    public class CalculatorGUI : Form
    {
        // Calculator controls
        readonly TextBox a = new TextBox { Left = 20, Top = 20, Width = 100 };
        readonly TextBox b = new TextBox { Left = 140, Top = 20, Width = 100 };
        readonly Button add = new Button { Left = 20, Top = 60, Width = 40, Text = "+" };
        readonly Button sub = new Button { Left = 70, Top = 60, Width = 40, Text = "−" };
        readonly Button mul = new Button { Left = 120, Top = 60, Width = 40, Text = "×" };
        readonly Button div = new Button { Left = 170, Top = 60, Width = 40, Text = "÷" };
        readonly Label result = new Label { Left = 20, Top = 100, AutoSize = true, Text = "Result: -" };

        // Unit Converter controls
        readonly Label convHeader = new Label { Left = 20, Top = 130, AutoSize = true, Text = "Unit Converter (miles/feet/cm)" };
        readonly Label valueLbl = new Label { Left = 20, Top = 160, AutoSize = true, Text = "Value:" };
        readonly TextBox convInput = new TextBox { Left = 70, Top = 158, Width = 100 };

        readonly Label fromLbl = new Label { Left = 180, Top = 160, AutoSize = true, Text = "From:" };
        readonly ComboBox fromUnit = new ComboBox { Left = 230, Top = 156, Width = 120, DropDownStyle = ComboBoxStyle.DropDownList };

        readonly Label toLbl = new Label { Left = 20, Top = 190, AutoSize = true, Text = "To:" };
        readonly ComboBox toUnit = new ComboBox { Left = 70, Top = 186, Width = 120, DropDownStyle = ComboBoxStyle.DropDownList };

        readonly Button convertBtn = new Button { Left = 200, Top = 185, Width = 80, Text = "Convert" };
        readonly Label convResult = new Label { Left = 290, Top = 190, AutoSize = true, Text = "Converted: -" };

        public CalculatorGUI()
        {
            Text = "Calculator + Unit Converter";
            Width = 460; Height = 280; FormBorderStyle = FormBorderStyle.FixedDialog; MaximizeBox = false;

            Controls.AddRange(new Control[] {
                a, b, add, sub, mul, div, result,
                convHeader, valueLbl, convInput,
                fromLbl, fromUnit, toLbl, toUnit,
                convertBtn, convResult
            });

            // Example: fromUnit.Items.Add("Miles");
            fromUnit.Items.Add("Miles");
            fromUnit.Items.Add("Feet");
            fromUnit.Items.Add("Centimetres");

            toUnit.Items.Add("Miles");
            toUnit.Items.Add("Feet");
            toUnit.Items.Add("Centimetres");

            // Hint: fromUnit.SelectedIndex = 0; toUnit.SelectedIndex = 1;
            fromUnit.SelectedIndex = 0;
            toUnit.SelectedIndex = 0;

            // Calculator button wiring (divide has a TODO inside)
            add.Click += (_, __) => ComputeAndShow((x, y) => x + y);
            sub.Click += (_, __) => ComputeAndShow((x, y) => x - y);
            mul.Click += (_, __) => ComputeAndShow((x, y) => x * y);
            div.Click += (_, __) =>
            {
                if (!TryRead(a, out var av) || !TryRead(b, out var bv)) return;
                //Handle divide by zero gracefully
                var r = (Math.Abs(bv) < double.Epsilon) ? double.NaN : av / bv;
                result.Text = double.IsNaN(r) ? "Result: NaN (divide-by-zero)" : $"Result: {r}";
            };

            // Hint: convertBtn.Click += (_, __) => will need to call a handler function
            convertBtn.Click += (_, __) => DoConvert();
        }

        void ComputeAndShow(Func<double, double, double> op)
        {
            if (!TryRead(a, out var av) || !TryRead(b, out var bv)) return;
            var r = op(av, bv);
            result.Text = $"Result: {r}";
        }

        bool TryRead(TextBox tb, out double v)
        {
            if (!double.TryParse(tb.Text, out v))
			{
				MessageBox.Show("Please enter a valid number.", "Input Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return false;
			}
			return true;
        }

        void DoConvert()
        {
            // Already implemented for you
            if (!TryRead(convInput, out var value)) return;
            int from = fromUnit.SelectedIndex;
            int to = toUnit.SelectedIndex;

            double resultVal = Native.ConvertUnits(value, from, to);
            convResult.Text = double.IsNaN(resultVal)
                ? "Converted: NaN"
                : $"Converted: {resultVal}";
        }
    }
}
