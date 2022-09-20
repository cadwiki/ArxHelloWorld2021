using System;
using System.Reflection;

namespace PlaceholderProject
{
    public class inheritedTestRunner : NUnitGui.TestRunner.NunitTestRunner
    {
        public void RunTests(Type[] regressionTestTypes)
        {
            base.Run(regressionTestTypes);
        }

	}
}
